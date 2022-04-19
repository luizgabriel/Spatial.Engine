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
    Type: ExportType.System,
    Query: {
        Required: ["Transform", "RotateVelocity"],
        Excluded: ["StopRotatingFlag"]
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
    Type: ExportType.Component,
    Properties: {
        Velocity: {
            Type: PropertyType.NumberRange,
            Default: 5.0,
            Min: .0,
            Max: 100.0,
        },
    },
};

const StopRotatingFlag = {
    Type: ExportType.ComponentFlag,
};

export default {RotateVelocity, StopRotatingFlag, RotateSystem};