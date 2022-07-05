//<editor-fold desc="import {...} from '@spatial/core';">
const updateComponents = (entity) => (components) => {
    return {
        namespace: "scene",
        type: "update-components",
        payload: {
            entity,
            components,
        },
    };
};

const log = (message) => {
    return {
        namespace: "editor",
        type: "log",
        payload: {
            message,
        },
    };
};

const PropertyType = Object.freeze({
    NumberRange: 'NumberRange',
    String: 'String',
});

const ExportType = Object.freeze({
    Component: 'Component',
    ComponentFlag: 'ComponentFlag',
    System: 'System',
});
//</editor-fold>

const RotateSystem = {
    type: ExportType.System,
    query: {
        required: ["Transform", "RotateVelocity"],
        excluded: ["StopRotatingFlag"]
    },
    onUpdateEntity: ({entity, delta, components}) => {
        const { velocity } = components["RotateVelocity"][entity];
        const transform = components["Transform"][entity];
        transform.rotation.x += velocity * delta;

        return [
            log(`A log message from entity ${entity}`),
            updateComponents(entity)({
                transform
            }),
        ];
    },
};

const RotateVelocity = {
    type: ExportType.Component,
    properties: {
        velocity: {
            type: PropertyType.NumberRange,
            default: 5.0,
            min: .0,
            max: 100.0,
        },
    },
};

const StopRotatingFlag = {
    type: ExportType.ComponentFlag,
};

export default {RotateVelocity, StopRotatingFlag, RotateSystem};